package com.example.elevadordafirma.ui.theme
import androidx.compose.material3.ColorScheme
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.darkColorScheme
import androidx.compose.runtime.Composable

private val ElevadorColorScheme = darkColorScheme(
    background = PretoFundo,
    surface = CinzaCard,
    primary = DouradoDetalhe,
    onPrimary = PretoFundo,
    onBackground = androidx.compose.ui.graphics.Color.White,
    onSurface = androidx.compose.ui.graphics.Color.White
)



@Composable
fun ElevadorTheme(content: @Composable () -> Unit) {
    MaterialTheme(
        colorScheme = ElevadorColorScheme,
        content = content
    )
}

