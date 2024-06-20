package com.labsigma.feederwebapp.entities;

import com.influxdb.annotations.Column;
import lombok.Data;

import java.time.Instant;

@Data
public class BirdFile {
    @Column(tag = true, name = "feeder_id")
    private String id;

    private Instant time;

    @Column(name = "file")
    private String fileName;

    @Column(name = "bird_name")
    private String birdName;

    @Column(name = "percent_prediction")
    private String percentPrediction;

    private String idFeeder;
}
