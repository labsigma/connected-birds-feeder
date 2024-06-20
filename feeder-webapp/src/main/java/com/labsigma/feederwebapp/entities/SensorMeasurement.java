package com.labsigma.feederwebapp.entities;


import com.influxdb.annotations.Column;
import lombok.Data;

import java.time.Instant;

@Data
public class SensorMeasurement {
    @Column(tag = true, name = "sensor_id")
    private String id;

    private Instant time;

    private Float temperature;
    private Float humidity;
    private Float co;
}
