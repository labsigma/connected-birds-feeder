package com.labsigma.feederwebapp.services;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.influxdb.client.*;
import com.labsigma.feederwebapp.entities.BirdFile;
import com.labsigma.feederwebapp.entities.Feeder;
import com.labsigma.feederwebapp.entities.SensorMeasurement;
import com.labsigma.feederwebapp.repositories.FeederRepository;
import lombok.RequiredArgsConstructor;
import lombok.extern.slf4j.Slf4j;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;

import java.time.OffsetDateTime;
import java.time.ZoneId;
import java.time.ZoneOffset;
import java.util.List;

@Service
@Slf4j
@RequiredArgsConstructor
public class FeederService {
    private final FeederRepository feederRepository;
    private final ObjectMapper objectMapper;
    private final ImageService imageService;
    private final static String PREFIX_ID = "IDFEEDER_";
    private final static String MEASUREMENT_SENSORS = "airSensors";
    private final static String MEASUREMENT_FEEDERS = "feeders";
    private final static String START_SENSORS = "-1d";
    private final static String START_FEEDERS = "0";
    private final static String SEPARATOR_QUERY = " |> ";

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
            filterId = String.format("filter(fn: (r) => r.feeder_id == \"%s\")", PREFIX_ID + idFeeder);
        }
        else {
            filterId = String.format("filter(fn: (r) => r.sensor_id == \"%s\")", PREFIX_ID + idFeeder);
        }

        return fromQuery + SEPARATOR_QUERY + rangeQuery + SEPARATOR_QUERY + filterMeasurement + SEPARATOR_QUERY +
                filterId + SEPARATOR_QUERY + "pivot(rowKey:[\"_time\"], columnKey: [\"_field\"], valueColumn: \"_value\")";
    }

    private InfluxDBClient getInfluxDBClient() {
        return InfluxDBClientFactory.create(
                InfluxDBClientOptions.builder()
                        .url(influxDbUrl)
                        .org(influxDbOrg)
                        .authenticateToken(influxDbToken.toCharArray())
                        .build()
        );
    }

    private void retrieveFeederInformation(Feeder feeder) {
        InfluxDBClient influxDBClient = getInfluxDBClient();
        QueryApi queryApi = influxDBClient.getQueryApi();

        List<BirdFile> birdFiles = queryApi.query(generateQuery(influxDbBucket, MEASUREMENT_FEEDERS, START_FEEDERS, feeder.getId().toString()), BirdFile.class);
        List<SensorMeasurement> sensorMeasurements = queryApi.query(generateQuery(influxDbBucket, MEASUREMENT_SENSORS, START_SENSORS, feeder.getId().toString()), SensorMeasurement.class);
        birdFiles.forEach(birdFile -> birdFile.setIdFeeder(feeder.getId().toString()));
        feeder.setBirdFiles(birdFiles);
        feeder.setSensorMeasurements(sensorMeasurements);

        influxDBClient.close();
    }

    public List<Feeder> retrieveFeeders() {
        List<Feeder> feeders = feederRepository.findAll();

        feeders.forEach(this::retrieveFeederInformation);

        return feeders;
    }

    public Boolean deleteFeeder(Long id) {
        log.info("Delete feeder with id " + id);
        try {
            feederRepository.findById(id).ifPresent(feederRepository::delete);
        }
        catch (Exception exception) {
            log.error(exception.getMessage());
            return false;
        }

        return true;
    }

    public Feeder addFeeder(Feeder feederParam) throws JsonProcessingException {
        log.info("Add Feeder : " +  objectMapper.writeValueAsString(feederParam));
        try {
            if (feederRepository.findById(feederParam.getId()).isPresent()) {
                log.error("The feeder with id " + feederParam.getId() + " already exist") ;
                return null;
            }
            else {
                Feeder feeder = new Feeder();
                feeder.setId(feederParam.getId());
                feeder.setDescription(feederParam.getDescription());
                feeder.setLongitude(feederParam.getLongitude());
                feeder.setLatitude(feederParam.getLatitude());

                feederRepository.saveAndFlush(feeder);
                retrieveFeederInformation(feeder);
                return feeder;
            }
        }
        catch (Exception exception) {
            log.error(exception.getMessage());
            return null;
        }
    }

    public Boolean updateFeeder(Feeder feederParam) throws JsonProcessingException {
        log.info("Update Feeder : " +  objectMapper.writeValueAsString(feederParam));
        try {
            Feeder feeder = feederRepository.findById(feederParam.getId()).orElse(null);
            if (feeder != null) {
                feeder.setDescription(feederParam.getDescription());
                feeder.setLongitude(feederParam.getLongitude());
                feeder.setLatitude(feederParam.getLatitude());

                feederRepository.save(feeder);
            }
            else {
                log.error("Feeder with id " + feederParam.getId() + " not found");
                return false;
            }
        }
        catch (Exception exception) {
            log.error(exception.getMessage());
            return false;
        }

        return true;
    }

    public Feeder getFeeder(Long id) {
        Feeder feeder = feederRepository.findById(id).orElse(null);
        if (feeder != null) {
            retrieveFeederInformation(feeder);
        }
        return feeder;
    }

    public Boolean deleteBirdFile(BirdFile birdFile) {
        InfluxDBClient influxDBClient = getInfluxDBClient();
        DeleteApi deleteApi = influxDBClient.getDeleteApi();

        String predicate = "_measurement=\"" + MEASUREMENT_FEEDERS + "\" AND feeder_id=\"" + birdFile.getId() + "\"";
        OffsetDateTime offsetDateTime = birdFile.getTime().atOffset(ZoneOffset.UTC);

        deleteApi.delete(offsetDateTime, offsetDateTime, predicate, influxDbBucket, influxDbOrg);

        influxDBClient.close();

        return imageService.deleteImage(birdFile.getIdFeeder(), birdFile.getFileName());
    }
}
