import {Component, OnInit} from '@angular/core';
import {Feeder} from '../../interfaces/feeder';

@Component({
  selector: 'app-galery',
  templateUrl: './gallery.component.html',
  styleUrls: ['./gallery.component.scss']
})
export class GalleryComponent implements OnInit {
  feeder: Feeder | undefined;

  constructor() {
  }
  ngOnInit(): void {
    const feeder = localStorage.getItem('feeder');
    if (feeder) {
      this.feeder = JSON.parse(feeder);
    }
  }

}
