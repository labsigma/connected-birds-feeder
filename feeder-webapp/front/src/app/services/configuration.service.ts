import { Injectable } from '@angular/core';
import {HttpClient, HttpErrorResponse} from '@angular/common/http';
import {Configuration} from '../interfaces/configuration';
import {BehaviorSubject} from 'rxjs';

@Injectable({
  providedIn: 'root'
})
export class ConfigurationService {
  private appConfig: Object | undefined;
  public configurationLoaded: BehaviorSubject<boolean> = new BehaviorSubject<boolean>(false);

  constructor(private http: HttpClient) { }

  async charger(): Promise<void> {
    await this.http.get('configuration')
      .toPromise()
      .then(
        (data) => {
          this.appConfig = data;
          this.configurationLoaded.next(true);
        },
        (error) => {
          if (error instanceof HttpErrorResponse) {
            if (error.error instanceof ErrorEvent) {
              console.error(error.url + ' => Error Event', error);
            }
          }
        }
      );
  }

  get configuration(): Configuration {
    return <Configuration>this.appConfig;
  }
}
