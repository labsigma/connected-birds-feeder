import {Component, Input, OnInit} from '@angular/core';
import {BirdFile} from '../../interfaces/bird-file';
import {ImageService} from '../../services/image.service';
import {Image} from '../../interfaces/image';

@Component({
  selector: 'app-bird',
  templateUrl: './bird.component.html',
  styleUrls: ['./bird.component.scss']
})
export class BirdComponent implements OnInit {
  @Input() birdFile!: BirdFile;

  constructor(private imageService: ImageService) {
  }

  ngOnInit(): void {
    if (this.birdFile) {
      this.imageService.getImage(this.birdFile.idFeeder, this.birdFile.fileName).subscribe((image: Image) => this.birdFile.src = image.src);
    }
  }
}
