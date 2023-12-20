import {Component, OnInit} from '@angular/core';
import {FeederService} from '../../services/feeder.service';
import {Feeder} from '../../interfaces/feeder';
import {LoaderService} from '../../services/loader.service';

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
    this.feederService.retreiveFeeders().subscribe((feeders: Feeder[]) => {
      try {
        this.feeders = feeders;
        this.feedersRetrieved = true;
      }
      finally {
        this.loaderService.hide();
      }
    },
      (err) => {
        console.error(err); // TODO afficher un message d'erreur
        this.loaderService.hide();
      });
  }



}
