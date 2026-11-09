package com.example.elevadordafirma.data.repository
import com.example.elevadordafirma.data.model.EstadoElevador
import android.util.Log
import com.hivemq.client.mqtt.datatypes.MqttQos
import com.hivemq.client.mqtt.mqtt3.Mqtt3Client
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.flow.asStateFlow
import kotlinx.coroutines.flow.update


class MqttRepository {

   private val _estado = MutableStateFlow(EstadoElevador())
   val estado: StateFlow<EstadoElevador> = _estado.asStateFlow()

   private val client = Mqtt3Client.builder()
      .identifier("CelularAndroid")
      .serverHost("192.168.0.228")
      .serverPort(1883)
      .automaticReconnectWithDefaultConfig()
      .addConnectedListener {
         Log.d("MQTT", "conectou")
         _estado.update { atual -> atual.copy(conectado = true) }
         ouvir()
      }
      .addDisconnectedListener {
         Log.d("MQTT", "desconectou")
         _estado.update { atual -> atual.copy(conectado = false) }
      }
      .buildAsync()

   fun conectar() {
      client.connectWith().send()
   }
   private fun ouvir() {
      for (topico in listOf("elevador/andar", "elevador/estado", "elevador/destino")) {
         client.subscribeWith()
            .topicFilter(topico)
            .callback { publish ->
               val texto = String(publish.payloadAsBytes)
               Log.d("MQTT", "$topico = $texto")
               guardar(topico, texto)
            }
            .send()
      }
   }

   private fun guardar(topico: String, texto: String) {
      _estado.update { atual ->
         when (topico) {
            "elevador/andar"   -> atual.copy(andar = texto)
            "elevador/estado"  -> atual.copy(estado = texto)
            "elevador/destino" -> atual.copy(destino = texto)
            else               -> atual
         }
      }
   }

   fun pedirAndar(andar: Int) {
      client.publishWith()
         .topic("elevador/comando")
         .payload(andar.toString().toByteArray())
         .qos(MqttQos.AT_LEAST_ONCE)
         .retain(false)
         .send()
   }
}