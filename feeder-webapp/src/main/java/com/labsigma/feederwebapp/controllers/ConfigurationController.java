package com.labsigma.feederwebapp.controllers;

import com.labsigma.feederwebapp.entities.Configuration;
import com.labsigma.feederwebapp.services.ConfigurationService;
import lombok.RequiredArgsConstructor;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
@RequestMapping("/configuration")
@RequiredArgsConstructor
public class ConfigurationController {
    private final ConfigurationService configurationService;

    @GetMapping
    public ResponseEntity<Configuration> getConfiguration() {
        Configuration configuration = new Configuration();
        configuration.setTitle(configurationService.getTitle());

        return ResponseEntity.ok(configuration);
    }
}
