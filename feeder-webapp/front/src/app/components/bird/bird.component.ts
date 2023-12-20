import {Component, Input, OnInit} from '@angular/core';
import {BirdFile} from '../../interfaces/bird-file';

@Component({
  selector: 'app-bird',
  templateUrl: './bird.component.html',
  styleUrls: ['./bird.component.scss']
})
export class BirdComponent {
  @Input() birdFile!: BirdFile;
}
