package com.labsigma.feederwebapp.properties;

import lombok.Data;
import org.springframework.boot.context.properties.EnableConfigurationProperties;
import org.springframework.context.annotation.Configuration;

@Configuration
@EnableConfigurationProperties
@org.springframework.boot.context.properties.ConfigurationProperties("configuration")
@Data
public class ConfigurationProperties {
    private String uploadDirectory;
    private String title;
}
