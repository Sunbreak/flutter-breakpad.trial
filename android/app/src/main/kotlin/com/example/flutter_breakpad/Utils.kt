package com.example.flutter_breakpad

class Utils {
    companion object {
        init {
            System.loadLibrary("flutter-breakpad");
        }
    }

    external fun crash()
}