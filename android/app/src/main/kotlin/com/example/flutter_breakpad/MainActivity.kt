package com.example.flutter_breakpad

import android.os.Handler
import io.flutter.embedding.android.FlutterActivity

class MainActivity: FlutterActivity() {
    private val utils = Utils()

    override fun onResume() {
        super.onResume()
        Handler().postDelayed({
            utils.crash()
        }, 1000)
    }
}
