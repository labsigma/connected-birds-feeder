import {Component, Input, OnInit} from '@angular/core';
import {Feeder} from '../../interfaces/feeder';
import {FeederService} from '../../services/feeder.service';
import {SensorMeasurement} from '../../interfaces/sensor-measurement';
import {Router} from '@angular/router';

@Component({
  selector: 'app-feeder',
  templateUrl: './feeder.component.html',
  styleUrls: ['./feeder.component.scss']
})
export class FeederComponent implements OnInit {
  @Input() feeder!: Feeder;

  lastTemperature = 0;
  lastHumidity = 0;
  lastCo = 0;

  constructor(
    private feederService: FeederService,
    private router: Router
  ) {
  }

  ngOnInit(): void {
    if (this.feeder) {
      const lastSensorMeasurement = this.feederService.getLastSensorMeasurement(this.feeder);
      if (lastSensorMeasurement) {
        this.lastTemperature = lastSensorMeasurement.temperature;
        this.lastHumidity = lastSensorMeasurement.humidity;
        this.lastCo = lastSensorMeasurement.co;
      }
    }
  }

  gotoGallery(feeder: Feeder): void {
    this.feederService.feedder = feeder;
    this.router.navigate(['/gallery']);
  }
}
