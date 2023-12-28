package com.labsigma.feederwebapp.repositories;

import com.labsigma.feederwebapp.entities.Feeder;
import org.jetbrains.annotations.NotNull;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;

import java.util.List;

@Repository
public interface FeederRepository extends JpaRepository<Feeder, Long> {

    @NotNull
    List<Feeder> findAll();
}
