import {Component, Input, OnInit} from '@angular/core';
import {Feeder} from '../../interfaces/feeder';
import {FeederService} from '../../services/feeder.service';

@Component({
  selector: 'app-galery',
  templateUrl: './gallery.component.html',
  styleUrls: ['./gallery.component.scss']
})
export class GalleryComponent implements OnInit {
  feeder: Feeder | undefined;

  constructor(private feederService: FeederService) {
  }
  ngOnInit(): void {
    this.feeder = this.feederService.feedder;
  }


}
