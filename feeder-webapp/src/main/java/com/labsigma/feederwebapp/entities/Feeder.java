package com.labsigma.feederwebapp.entities;

import lombok.Data;

import java.util.ArrayList;
import java.util.List;

@Data
public class Feeder {
    private Long id;
    private String description;
    private Float longitude;
    private Float latitude;
    private List<SensorMeasurement> sensorMeasurements;
    private List<BirdFile> birdFiles;

    public Feeder() {
        sensorMeasurements = new ArrayList<>();
        birdFiles = new ArrayList<>();
    }
}
