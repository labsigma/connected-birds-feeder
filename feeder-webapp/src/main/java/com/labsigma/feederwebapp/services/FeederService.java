package com.labsigma.feederwebapp.services;

import com.influxdb.client.*;
import com.influxdb.client.domain.WritePrecision;
import com.influxdb.query.FluxRecord;
import com.influxdb.query.FluxTable;
import com.labsigma.feederwebapp.entities.BirdFile;
import com.labsigma.feederwebapp.entities.Feeder;
import com.labsigma.feederwebapp.entities.SensorMeasurement;
import com.labsigma.feederwebapp.properties.ConfigurationProperties;
import lombok.RequiredArgsConstructor;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;

import java.util.ArrayList;
import java.util.List;

@Service
@RequiredArgsConstructor
public class FeederService {
    private final ConfigurationProperties configurationProperties;

    private static String PREFIXE_ID = "IDFEEDER_";
    private static String MEASUREMENT_AIRSENSORS = "airSensors";
    private static String MEASUREMENT_FEEDERS = "feeders";
    private static String START_AIRSENSORS = "-1d";
    private static String START_FEEDERS = "0";
    private static String SEPARATOR_QUERY = " |> ";

    @Value("${influxDb.url}")
    String influxDbUrl;

    @Value("${influxDb.organisation}")
    String influxDbOrg;

    @Value("${influxDb.bucket}")
    String influxDbBucket;

    @Value("${influxDb.token}")
    String influxDbToken;

    private String generateQuery(String bucket, String typeMeasurement, String start, String idFeeder) {
        String fromQuery = String.format("from(bucket:\"%s\")", bucket);
        String rangeQuery = String.format("range(start: %s)", start);
        String filterMeasurement = String.format("filter(fn: (r) => r._measurement == \"%s\")", typeMeasurement);
        String filterId;
        if (typeMeasurement.equals(MEASUREMENT_FEEDERS)) {
            filterId = String.format("filter(fn: (r) => r.feeder_id == \"%s\")", PREFIXE_ID + idFeeder);
        }
        else {
            filterId = String.format("filter(fn: (r) => r.sensor_id == \"%s\")", PREFIXE_ID + idFeeder);
        }

        return fromQuery + SEPARATOR_QUERY + rangeQuery + SEPARATOR_QUERY + filterMeasurement + SEPARATOR_QUERY +
                filterId + SEPARATOR_QUERY + "pivot(rowKey:[\"_time\"], columnKey: [\"_field\"], valueColumn: \"_value\")";
    }

    public List<Feeder> retreiveFeeders() {
        List<Feeder> feeders = configurationProperties.getFeeders();

        InfluxDBClient influxDBClient = InfluxDBClientFactory.create(
                InfluxDBClientOptions.builder()
                        .url(influxDbUrl)
                        .org(influxDbOrg)
                        .authenticateToken(influxDbToken.toCharArray())
                        .build()
        );

        QueryApi queryApi = influxDBClient.getQueryApi();

        feeders.forEach(feeder -> {
            List<BirdFile> birdFiles = queryApi.query(generateQuery(influxDbBucket, MEASUREMENT_FEEDERS, START_FEEDERS, feeder.getId()), BirdFile.class);
            List<SensorMeasurement> sensorMeasurements = queryApi.query(generateQuery(influxDbBucket, MEASUREMENT_AIRSENSORS, START_AIRSENSORS, feeder.getId()), SensorMeasurement.class);
            feeder.setBirdFiles(birdFiles);
            // update updload directory for each bird file
            feeder.getBirdFiles().forEach(birdFile -> {
                birdFile.setUpdloadDirectory(configurationProperties.getUpdloadDirectory() + "/" + feeder.getId() + "/");
            });
            feeder.setSensorMeasurements(sensorMeasurements);
        });

        influxDBClient.close();

        return feeders;
    }
}
