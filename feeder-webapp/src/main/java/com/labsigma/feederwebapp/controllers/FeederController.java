package com.labsigma.feederwebapp.controllers;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.labsigma.feederwebapp.entities.Feeder;
import com.labsigma.feederwebapp.services.FeederService;
import lombok.RequiredArgsConstructor;
import org.springframework.web.bind.annotation.*;

import java.util.List;

@RestController
@RequestMapping("/api")
@RequiredArgsConstructor
public class FeederController {
    private final FeederService feederService;

    @GetMapping(value = "/feeders")
    public List<Feeder> retrieveFeeders() {
        return feederService.retrieveFeeders();
    }

    @PostMapping(value = "/feeder")
    public @ResponseBody Feeder addFeeder(@RequestBody Feeder feeder) throws JsonProcessingException {
        return feederService.addFeeder(feeder);
    }

    @PutMapping(value = "/feeder")
    public @ResponseBody Boolean updateFeeder(@RequestBody Feeder feeder) throws JsonProcessingException {
        return feederService.updateFeeder(feeder);
    }

    @DeleteMapping(value = "/feeder/{id}")
    public @ResponseBody Boolean deleteFeeder(@PathVariable Long id) {
        return feederService.deleteFeeder(id);
    }

    @GetMapping(value = "/feeder/{id}")
    public Feeder getFeeder(@PathVariable Long idFeeder) {
        return feederService.getFeeder(idFeeder);
    }
}
