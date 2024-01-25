import {Component, OnInit} from '@angular/core';
import {Feeder} from '../../interfaces/feeder';
import {Title} from '@angular/platform-browser';
import {TranslateService} from '@ngx-translate/core';
import {FeederService} from '../../services/feeder.service';
import {BirdFile} from '../../interfaces/bird-file';

@Component({
  selector: 'app-gallery',
  templateUrl: './gallery.component.html',
  styleUrls: ['./gallery.component.scss']
})
export class GalleryComponent implements OnInit {
  feeder: Feeder | undefined;

  constructor(
    private feederService: FeederService

  ) {
  }
  ngOnInit(): void {
    const feeder = localStorage.getItem('feeder');
    if (feeder) {
      this.feeder = JSON.parse(feeder);
    }

    this.feederService.birdFileDeleted.subscribe((birdFile: BirdFile) => {
      if (this.feeder) {
        const index = this.feeder.birdFiles.findIndex(b => b === birdFile);
        if (index !== -1) {
          this.feeder.birdFiles.splice(index, 1);
        }
      }
    });
  }

}
