package com.labsigma.feederwebapp.controllers;

import com.labsigma.feederwebapp.entities.Image;
import com.labsigma.feederwebapp.services.ImageService;
import lombok.RequiredArgsConstructor;
import org.springframework.http.MediaType;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.awt.*;
import java.io.IOException;

@RestController
@RequestMapping("/image")
@RequiredArgsConstructor
public class ImageController {
    private final ImageService imageService;

    @GetMapping()
    public Image getImage(@RequestParam String idFeeder, @RequestParam String imageName) throws IOException {
        return imageService.getImage(idFeeder, imageName);
    }
}
