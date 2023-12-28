import { Injectable } from '@angular/core';
import {
  Router, Resolve,
  RouterStateSnapshot,
  ActivatedRouteSnapshot
} from '@angular/router';
import { Observable, of } from 'rxjs';
import {TranslateService} from '@ngx-translate/core';

@Injectable({
  providedIn: 'root'
})
export class CustomTitleResolver implements Resolve<string> {
  constructor(
    private translateService: TranslateService,
  ) {}

  resolve(route: ActivatedRouteSnapshot): Promise<string> {
    if (!route.data['titleKey']) {
      return Promise.resolve('Le Lab SIGMA - Mangeoires connectées');
    }

    return Promise.resolve(
      this.translateService.instant(route.data['titleKey'])
    );
  }
}
