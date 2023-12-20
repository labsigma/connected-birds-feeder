import {SensorMeasurement} from './sensor-measurement';
import {BirdFile} from './bird-file';

export interface Feeder {
  id: number;
  description: string;
  longitude: number;
  latitude: number;
  sensorMeasurements: SensorMeasurement[];
  birdFiles: BirdFile[];
}
