import {Component, OnInit} from '@angular/core';
import {Feeder} from '../../interfaces/feeder';
import {Title} from '@angular/platform-browser';
import {TranslateService} from '@ngx-translate/core';

@Component({
  selector: 'app-gallery',
  templateUrl: './gallery.component.html',
  styleUrls: ['./gallery.component.scss']
})
export class GalleryComponent implements OnInit {
  feeder: Feeder | undefined;

  constructor(
    private titleService: Title,
    private translateService: TranslateService
  ) {
  }
  ngOnInit(): void {
    this.titleService.setTitle(this.translateService.instant('titre.gallerie'));
    const feeder = localStorage.getItem('feeder');
    if (feeder) {
      this.feeder = JSON.parse(feeder);
    }
  }

}
