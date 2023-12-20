package com.labsigma.feederwebapp.entities;


import com.influxdb.annotations.Column;
import lombok.Getter;
import lombok.Setter;

import java.time.Instant;

@Getter
@Setter
public class SensorMeasurement {
    @Column(tag = true, name = "sensor_id")
    private String id;

    private Instant time;

    private Float temperature;
    private Float humidity;
    private Float co;
}
