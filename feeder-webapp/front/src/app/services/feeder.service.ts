import { Injectable } from '@angular/core';
import {HttpClient} from '@angular/common/http';
import {BehaviorSubject, Observable} from 'rxjs';
import {Feeder} from '../interfaces/feeder';
import {SensorMeasurement} from '../interfaces/sensor-measurement';

@Injectable({
  providedIn: 'root'
})
export class FeederService {
  constructor(private httpClient: HttpClient) {}

  public feederDeleted = new BehaviorSubject<any>(undefined);
  public feederUpdated = new BehaviorSubject<any>(undefined);

  public retrieveFeeders(): Observable<Feeder[]> {
    return this.httpClient.get<Feeder[]>('api/feeders');
  }

  private sortSensorMeasurementsByTime(s1: SensorMeasurement, s2: SensorMeasurement): number {
    if (s1.time > s2.time) {
      return -1;
    }
    else if (s1.time < s2.time) {
      return 1;
    }

    return 0;
  }

  public sortFeeders(f1: Feeder, f2: Feeder): number {
    return f1.description.localeCompare(f2.description);
  }

  public getLastSensorMeasurement(feeder: Feeder): SensorMeasurement | undefined {
    let sensorMeasurement;

    if (feeder.sensorMeasurements.length > 0) {
      sensorMeasurement = feeder.sensorMeasurements.sort(this.sortSensorMeasurementsByTime).at(0);
    }

    return sensorMeasurement;
  }

  public addFeeder(feeder: Feeder): Observable<Feeder> {
    return this.httpClient.post<Feeder>('api/feeder', feeder);
  }

  public updateFeeder(feeder: Feeder): Observable<boolean> {
    return this.httpClient.put<boolean>('api/feeder', feeder);
  }

  public deleteFeeder(feederId: number): Observable<boolean> {
    return this.httpClient.delete<boolean>('api/feeder/' + feederId);
  }

  public feederWasDeleted(feeder: Feeder): void {
    this.feederDeleted.next(feeder);
  }

  public feederWasUpdated(feeder: Feeder): void {
    this.feederUpdated.next(feeder);
  }

}
