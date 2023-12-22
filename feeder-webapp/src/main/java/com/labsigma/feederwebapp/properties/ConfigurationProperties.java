package com.labsigma.feederwebapp.properties;

import com.labsigma.feederwebapp.entities.Feeder;
import lombok.Getter;
import lombok.Setter;
import org.springframework.boot.context.properties.EnableConfigurationProperties;
import org.springframework.context.annotation.Configuration;

import java.util.List;

@Configuration
@EnableConfigurationProperties
@org.springframework.boot.context.properties.ConfigurationProperties("configuration")
@Getter
@Setter
public class ConfigurationProperties {
    private String uploadDirectory;
    private List<Feeder> feeders;
}
