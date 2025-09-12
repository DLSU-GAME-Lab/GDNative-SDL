package org.main.babaylan_tales;

import android.os.Bundle;

import org.libsdl.app.SDLActivity;

public class Babaylan_Tales extends SDLActivity {


    // Do code here like check permissions or something if needed
    @Override
    public void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
    }
    @Override
    public String[] getLibraries() {
        return new String[]{
                "Babaylan_Tales" // This is the name of the native library that will be loaded
        };

    }
}

