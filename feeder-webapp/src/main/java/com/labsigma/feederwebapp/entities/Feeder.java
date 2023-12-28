package com.labsigma.feederwebapp.entities;

import jakarta.persistence.Entity;
import jakarta.persistence.Id;
import jakarta.persistence.Transient;
import lombok.Getter;
import lombok.Setter;
import org.springframework.data.relational.core.mapping.Column;
import org.springframework.data.relational.core.mapping.Table;

import java.util.ArrayList;
import java.util.List;

@Entity
@Getter
@Setter
@Table(name = "FEEDER")
public class Feeder {
    @Column("id")
    @Id
    private Long id;

    @Column("description")
    private String description;

    @Column("longitude")
    private Float longitude;

    @Column("latitude")
    private Float latitude;

    @Transient
    private List<SensorMeasurement> sensorMeasurements;

    @Transient
    private List<BirdFile> birdFiles;

    public Feeder() {
        sensorMeasurements = new ArrayList<>();
        birdFiles = new ArrayList<>();
    }
}
