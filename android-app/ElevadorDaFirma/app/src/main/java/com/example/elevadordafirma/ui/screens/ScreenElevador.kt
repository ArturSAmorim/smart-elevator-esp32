package com.example.elevadordafirma.ui.screens

import androidx.compose.foundation.BorderStroke
import androidx.compose.foundation.background
import androidx.compose.foundation.layout.*
import androidx.compose.foundation.shape.CircleShape
import androidx.compose.foundation.shape.RoundedCornerShape
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.ArrowDownward
import androidx.compose.material.icons.filled.ArrowUpward
import androidx.compose.material.icons.filled.Settings
import androidx.compose.material3.*
import androidx.compose.runtime.*
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.text.font.FontWeight
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.sp
import androidx.lifecycle.viewmodel.compose.viewModel
import com.example.elevadordafirma.ui.theme.CinzaTexto
import com.example.elevadordafirma.ui.theme.DouradoDetalhe
import com.example.elevadordafirma.ui.theme.ElevadorTheme
import com.example.elevadordafirma.ui.theme.VermelhoOffline
import com.example.elevadordafirma.ui.viewmodel.ElevadorViewModel

@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun ScreenElevador(vm: ElevadorViewModel = viewModel()) {

    val estado by vm.estado.collectAsState()

    ElevadorTheme {
        Column(
            modifier = Modifier
                .fillMaxSize()
                .background(MaterialTheme.colorScheme.background)
                .padding(24.dp),
            horizontalAlignment = Alignment.CenterHorizontally
        ) {
            Row(
                modifier = Modifier.fillMaxWidth(),
                horizontalArrangement = Arrangement.SpaceBetween,
                verticalAlignment = Alignment.CenterVertically
            ) {
                Text(
                    text = "Controle do Elevador",
                    color = Color.White,
                    fontSize = 20.sp,
                    fontWeight = FontWeight.Bold
                )

                val corStatus = if (estado.conectado) DouradoDetalhe else VermelhoOffline
                val textoStatus = if (estado.conectado) "Online" else "Offline"

                Surface(
                    shape = RoundedCornerShape(16.dp),
                    color = corStatus.copy(alpha = 0.1f),
                    border = BorderStroke(1.dp, corStatus.copy(alpha = 0.3f))
                ) {
                    Row(
                        modifier = Modifier.padding(horizontal = 12.dp, vertical = 6.dp),
                        verticalAlignment = Alignment.CenterVertically
                    ) {
                        Box(
                            modifier = Modifier
                                .size(8.dp)
                                .background(corStatus, CircleShape)
                        )
                        Spacer(modifier = Modifier.width(6.dp))
                        Text(text = textoStatus, color = corStatus, fontSize = 12.sp)
                    }
                }
            }

            Spacer(modifier = Modifier.height(40.dp))
            Surface(
                modifier = Modifier
                    .fillMaxWidth()
                    .aspectRatio(1f),
                shape = RoundedCornerShape(24.dp),
                color = MaterialTheme.colorScheme.surface,
                shadowElevation = 8.dp
            ) {
                Column(
                    modifier = Modifier.fillMaxSize(),
                    horizontalAlignment = Alignment.CenterHorizontally,
                    verticalArrangement = Arrangement.Center
                ) {
                    Text(
                        text = "ANDAR ATUAL",
                        color = CinzaTexto,
                        fontSize = 14.sp,
                        letterSpacing = 1.5.sp
                    )

                    Text(
                        text = nomeAndar(estado.andar),
                        color = Color.White,
                        fontSize = 110.sp,
                        fontWeight = FontWeight.Black
                    )

                    Surface(
                        shape = RoundedCornerShape(16.dp),
                        color = Color.Transparent,
                        border = BorderStroke(1.dp, DouradoDetalhe)
                    ) {
                        Row(
                            modifier = Modifier.padding(horizontal = 16.dp, vertical = 8.dp),
                            verticalAlignment = Alignment.CenterVertically
                        ) {
                            Text(
                                text = estado.estado,
                                color = DouradoDetalhe,
                                fontWeight = FontWeight.Medium
                            )
                            Spacer(modifier = Modifier.width(4.dp))

                            val icone = when (estado.estado.lowercase()) {
                                "subindo" -> Icons.Default.ArrowUpward
                                "descendo" -> Icons.Default.ArrowDownward
                                else -> Icons.Default.Settings // Ícone para parado/calibrando
                            }
                            Icon(icone, contentDescription = null, tint = DouradoDetalhe, modifier = Modifier.size(16.dp))
                        }
                    }
                }
            }

            Spacer(modifier = Modifier.height(42.dp))

            Text(
                text = "SELECIONE O DESTINO",
                color = CinzaTexto,
                fontSize = 12.sp,
                letterSpacing = 1.sp
            )

            Spacer(modifier = Modifier.height(24.dp))

            Row(
                modifier = Modifier.fillMaxWidth(),
                horizontalArrangement = Arrangement.SpaceEvenly
            ) {
                listOf(0 to "T", 1 to "1", 2 to "2", 3 to "3").forEach { (numAndar, rotulo) ->
                    val isDestino = estado.destino == numAndar.toString()

                    BotaoRedondoAndar(
                        texto = rotulo,
                        ativo = isDestino,
                        habilitado = estado.conectado,
                        onClick = { vm.pedirAndar(numAndar) }
                    )
                }
            }
        }
    }
}


private fun nomeAndar(texto: String) = if (texto == "0") "T" else texto

@Composable
fun BotaoRedondoAndar(texto: String, ativo: Boolean, habilitado: Boolean, onClick: () -> Unit) {
    val corFundo = if (ativo) DouradoDetalhe.copy(alpha = 0.15f) else Color(0xFF2A2A2A)
    val corBorda = if (ativo) DouradoDetalhe else Color(0xFF3D3D3D)
    val corTexto = if (ativo) DouradoDetalhe else if (habilitado) Color.White else CinzaTexto.copy(alpha = 0.5f)

    Surface(
        onClick = onClick,
        enabled = habilitado,
        shape = CircleShape,
        color = corFundo,
        border = BorderStroke(if (ativo) 2.dp else 1.dp, corBorda),
        modifier = Modifier.size(72.dp)
    ) {
        Box(contentAlignment = Alignment.Center) {
            Text(
                text = texto,
                color = corTexto,
                fontSize = 28.sp,
                fontWeight = FontWeight.Bold
            )
        }
    }
}