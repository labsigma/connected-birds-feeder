import { Injectable } from '@angular/core';
import {NgxSpinnerService} from 'ngx-spinner';

@Injectable({
  providedIn: 'root'
})
export class LoaderService {

  constructor(private spinnerService: NgxSpinnerService) { }

  public show(): void {
    this.spinnerService.show('app-loader', {type: 'ball-spin-clockwise'});
  }

  public hide(): void {
    this.spinnerService.hide('app-loader');
  }
}
