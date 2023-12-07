package com.labsigma.feederwebapp.controllers;

import com.labsigma.feederwebapp.entities.Feeder;
import com.labsigma.feederwebapp.services.FeederService;
import lombok.RequiredArgsConstructor;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import java.util.List;

@RestController
@RequestMapping("/api")
@RequiredArgsConstructor
public class FeederController {
    private final FeederService feederService;

    @GetMapping(value = "/feeders")
    public List<Feeder> retreiveFeeders() {
        return feederService.retreiveFeeders();
    }
}
