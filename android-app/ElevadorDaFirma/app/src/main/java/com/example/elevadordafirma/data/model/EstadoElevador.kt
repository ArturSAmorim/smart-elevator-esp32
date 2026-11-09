package com.example.elevadordafirma.data.model

data class EstadoElevador(
    val conectado: Boolean = false,
    val andar: String = "-",
    val estado: String = "-",
    val destino: String = "-"
)