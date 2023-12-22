import { Injectable } from '@angular/core';
import {HttpClient} from '@angular/common/http';
import {Observable} from 'rxjs';
import {Feeder} from '../interfaces/feeder';
import {SensorMeasurement} from '../interfaces/sensor-measurement';

@Injectable({
  providedIn: 'root'
})
export class FeederService {
  constructor(private httpClient: HttpClient) {}

  public retreiveFeeders(): Observable<Feeder[]> {
    return this.httpClient.get<Feeder[]>('api/feeders');
  }

  private sortSensorMesasurementByTime(s1: SensorMeasurement, s2: SensorMeasurement): number {
    if (s1.time > s2.time) {
      return -1;
    }
    else if (s1.time < s2.time) {
      return 1;
    }

    return 0;
  }

  public getLastSensorMeasurement(feeder: Feeder): SensorMeasurement | undefined {
    let sensorMeasurement;

    if (feeder.sensorMeasurements.length > 0) {
      sensorMeasurement = feeder.sensorMeasurements.sort(this.sortSensorMesasurementByTime).at(0);
    }

    return sensorMeasurement;
  }



}
