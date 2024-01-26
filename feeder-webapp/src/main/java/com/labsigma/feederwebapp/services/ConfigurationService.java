package com.labsigma.feederwebapp.services;

import com.labsigma.feederwebapp.properties.ConfigurationProperties;
import lombok.RequiredArgsConstructor;
import org.springframework.stereotype.Service;

@Service
@RequiredArgsConstructor
public class ConfigurationService {
    private final ConfigurationProperties configurationProperties;

    public String getUploadDirectory() {
        return configurationProperties.getUploadDirectory();
    }

    public String getTitle() {
        return configurationProperties.getTitle();
    }
}
