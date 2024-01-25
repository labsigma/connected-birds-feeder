import {Component, Input, OnInit} from '@angular/core';
import {BirdFile} from '../../interfaces/bird-file';
import {ImageService} from '../../services/image.service';
import {Image} from '../../interfaces/image';
import {FeederService} from '../../services/feeder.service';
import {ToastrService} from 'ngx-toastr';
import {TranslateService} from '@ngx-translate/core';

@Component({
  selector: 'app-bird',
  templateUrl: './bird.component.html',
  styleUrls: ['./bird.component.scss']
})
export class BirdComponent implements OnInit {
  @Input() birdFile!: BirdFile;

  constructor(
    private imageService: ImageService,
    private feederService: FeederService,
    private toastrService: ToastrService,
    private translateService: TranslateService
  ) {
  }

  ngOnInit(): void {
    if (this.birdFile) {
      this.imageService.getImage(this.birdFile.idFeeder, this.birdFile.fileName).subscribe(
        (image: Image) => {
          let src = image.src;
          if (!src) {
            src = 'assets/img/no-image.png';
          }
          this.birdFile.src = src;

        }
      );
    }
  }

  delete(birdFile: BirdFile): void {
    this.feederService.deleteBirdFile(birdFile).subscribe(
      (result: boolean) => {
        if (result) {
          this.feederService.birdFileWasDeleted(birdFile);
        }
        else {
          this.toastrService.show(this.translateService.instant('erreur.suppression-image'));
        }
      },
      (error) => {
        console.error(error);
        this.toastrService.show(this.translateService.instant('erreur.suppression-image'));
      }
    );
  }
}
