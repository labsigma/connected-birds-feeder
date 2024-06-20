package com.labsigma.feederwebapp.repositories;

import com.fasterxml.jackson.core.type.TypeReference;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.labsigma.feederwebapp.entities.Feeder;
import lombok.RequiredArgsConstructor;
import org.springframework.stereotype.Repository;

import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;
import java.util.Optional;

@Repository
@RequiredArgsConstructor
public class FeederRepository {
    private static final String FEEDERS_FILE_NAME = "feeders.json";
    private final ObjectMapper objectMapper;
    private List<Feeder> feeders;

    public List<Feeder> findAll() throws IOException {
        feeders = new ArrayList<>();

        // Load the feeders from internal json file
        Path path = Paths.get(FEEDERS_FILE_NAME);
        if (!Files.exists(path)) {
            Files.createFile(path);
            saveFile();
        }

        File feedersFile = new File(FEEDERS_FILE_NAME);

        if (feedersFile.exists()) {
            feeders = objectMapper.readValue(feedersFile, new TypeReference<>() {});
        }

        return feeders;
    }

    public Optional<Feeder> findById(Long id) {
        return feeders.stream().filter(feeder -> feeder.getId().equals(id)).findFirst();
    }

    public void modify(Feeder feeder) throws IOException {
        // modify feeder in the list of feeder and save the file
        int index = feeders.indexOf(feeder);
        if (index != -1) {
            feeders.set(index, feeder);
        }
        saveFile();
    }

    public void delete(Feeder feeder) throws IOException {
        // remove feeder in th list and save the file
        feeders.remove(feeder);
        saveFile();
    }

    private void saveFile() throws IOException {
        objectMapper.writeValue(new File(FEEDERS_FILE_NAME), feeders);
    }

    public void create(Feeder feeder) throws IOException {
        // Add feeder and save file
        feeders.add(feeder);
        saveFile();
    }
}
