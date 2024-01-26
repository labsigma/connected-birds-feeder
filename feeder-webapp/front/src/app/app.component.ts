import {Component, OnInit} from '@angular/core';
import {TranslateService} from '@ngx-translate/core';
import {Title} from '@angular/platform-browser';
import {ConfigurationService} from './services/configuration.service';

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.scss']
})
export class AppComponent implements OnInit {

  constructor(
    private configurationService: ConfigurationService,
    private titleService: Title
  ) {
  }
  ngOnInit(): void {
    this.configurationService.configurationLoaded.subscribe(
      (result: boolean) => {
        if (result) {
          this.titleService.setTitle(this.configurationService.configuration.title);
        }
      }
    )
  }
}
