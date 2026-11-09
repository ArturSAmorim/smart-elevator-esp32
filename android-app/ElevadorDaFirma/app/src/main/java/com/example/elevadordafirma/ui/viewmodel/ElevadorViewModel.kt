package com.example.elevadordafirma.ui.viewmodel

import androidx.lifecycle.ViewModel
import com.example.elevadordafirma.data.model.EstadoElevador
import com.example.elevadordafirma.data.repository.MqttRepository
import kotlinx.coroutines.flow.StateFlow

class
ElevadorViewModel : ViewModel() {

    private val repository = MqttRepository()
    val estado: StateFlow<EstadoElevador> = repository.estado

    init {
        repository.conectar()
    }

    fun pedirAndar(andar: Int) {
        repository.pedirAndar(andar)
    }
}