package com.labsigma.feederwebapp.entities;

import com.influxdb.annotations.Column;
import lombok.Getter;
import lombok.Setter;

import java.time.Instant;

@Getter
@Setter
public class BirdFile {
    @Column(tag = true, name = "feeder_id")
    private String id;

    private Instant time;

    private String updloadDirectory;

    @Column(name = "file")
    private String fileName;

    @Column(name = "bird_name")
    private String birdName;

    @Column(name = "percent_prediction")
    private String percentPrediction;
}
