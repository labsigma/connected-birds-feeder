import {Component, OnInit} from '@angular/core';
import {FeederService} from '../../services/feeder.service';
import {Feeder} from '../../interfaces/feeder';
import {LoaderService} from '../../services/loader.service';
import {Title} from '@angular/platform-browser';
import {TranslateService} from '@ngx-translate/core';

@Component({
  selector: 'app-feeders',
  templateUrl: './feeders.component.html',
  styleUrls: ['./feeders.component.scss']
})
export class FeedersComponent implements OnInit {

  constructor(
    private feederService: FeederService,
    private loaderService: LoaderService
  ) {
  }

  feeders: Feeder[] = [];
  feedersRetrieved = false;

  ngOnInit(): void {
    this.loaderService.show();
    this.feederService.retrieveFeeders().subscribe({
      next: (feeders: Feeder[]) => {
        try {
          this.feeders = feeders.sort(this.feederService.sortFeeders);
          this.feedersRetrieved = true;
        }
        finally {
          this.loaderService.hide();
        }
      },
      error: (err) => {
        console.error(err);
        this.loaderService.hide();
      }
    });

    this.feederService.feederDeleted.subscribe((feeder: Feeder) => {
      if (feeder) {
        const index = this.feeders.findIndex((f: Feeder) => f.id === feeder.id);
        if (index !== -1) {
          this.feeders.splice(index,1);
        }
      }
    });

  }

}
