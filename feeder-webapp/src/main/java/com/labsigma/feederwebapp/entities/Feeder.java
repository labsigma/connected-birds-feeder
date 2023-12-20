package com.labsigma.feederwebapp.entities;

import lombok.Getter;
import lombok.Setter;

import java.util.ArrayList;
import java.util.List;

@Getter
@Setter
public class Feeder {
    private String id;
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
