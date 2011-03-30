package org.trollop.Plot;

import java.util.Random;
import java.util.SortedMap;
import java.util.TreeMap;

import android.app.Activity;
import android.os.Bundle;

public class Plotting extends Activity {
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        //        X-axis Y-axis
        SortedMap<Float, Float> floatPoints = new TreeMap<Float, Float>();
        
        Random randomGenerator = new Random();
        for (Float i = 0F; i < 20; i++) {
        	floatPoints.put(randomGenerator.nextFloat(), randomGenerator.nextFloat());
        }
        floatPoints.clear();
        
        float fl = -1.425F;
        float interval = 0.2F;
        floatPoints.put(fl += interval, 0.000F);
        floatPoints.put(fl += interval, -0.089F);
        floatPoints.put(fl += interval, 0.070F);
        floatPoints.put(fl += interval, 0.046F);
        floatPoints.put(fl += interval, 0.058F);
        floatPoints.put(fl += interval, 0.067F);
        floatPoints.put(fl += interval, 0.028F);
        floatPoints.put(fl += interval, 0.068F);
        floatPoints.put(fl += interval, 0.084F);
        floatPoints.put(fl += interval, 0.031F);
        floatPoints.put(fl += interval, 0.173F);
        floatPoints.put(fl += interval, 0.064F);
        floatPoints.put(fl += interval, 0.089F);
        floatPoints.put(fl += interval, 0.051F);
        floatPoints.put(fl += interval, 0.047F);
        floatPoints.put(fl += interval, 0.071F);
        floatPoints.put(fl += interval, 0.007F);
        floatPoints.put(fl += interval, 0.015F);
        floatPoints.put(fl += interval, 0.058F);
        floatPoints.put(fl += interval, 0.018F);
        floatPoints.put(fl += interval, -0.068F);
        
        Chart chart = new Chart(getApplicationContext(),
        		floatPoints,
        		"Fancy Chart of Some Kind",
        		"Time",
        		"Power",
        		"Seconds",
        		"Volts",
        		true);
        
        setContentView(chart);
    }
}