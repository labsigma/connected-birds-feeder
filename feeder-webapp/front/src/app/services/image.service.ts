import { Injectable } from '@angular/core';
import {HttpClient} from '@angular/common/http';
import {Observable} from 'rxjs';
import {Image} from '../interfaces/image';

@Injectable({
  providedIn: 'root'
})
export class ImageService {

  constructor(private httpClient: HttpClient) { }

  public getImage(idFeeder: string, imageName: string): Observable<Image> {
    const url = 'image?idFeeder=' + idFeeder + '&imageName=' + imageName;
    return this.httpClient.get<Image>(url);
  }

}
