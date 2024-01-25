package com.labsigma.feederwebapp.services;

import com.labsigma.feederwebapp.entities.Image;
import com.labsigma.feederwebapp.properties.ConfigurationProperties;
import lombok.RequiredArgsConstructor;
import org.apache.commons.io.FileUtils;
import org.springframework.stereotype.Service;

import java.io.File;
import java.io.IOException;
import java.util.Base64;

@Service
@RequiredArgsConstructor
public class ImageService {
    private final ConfigurationProperties configurationProperties;

    public Image getImage(String idFeeder, String imageName) throws IOException {
        // Get the image path
        String imagePath = configurationProperties.getUploadDirectory() + "/" + idFeeder + "/" + imageName;
        Image image = new Image();

        File file = new File(imagePath);
        if (file.exists()) {
            byte[] fileContent = FileUtils.readFileToByteArray(new File(imagePath));
            image.setSrc("data:image/jpeg;base64," + Base64.getEncoder().encodeToString(fileContent));
        }

        return image;
    }

    public Boolean deleteImage(String idFeeder, String imageName) {
        String imagePath = configurationProperties.getUploadDirectory() + "/" + idFeeder + "/" + imageName;
        File file = new File(imagePath);
        if (file.exists()) {
            return file.delete();
        }

        return true;
    }
}
