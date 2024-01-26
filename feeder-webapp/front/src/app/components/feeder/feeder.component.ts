import {Component, Input, OnInit} from '@angular/core';
import {Feeder} from '../../interfaces/feeder';
import {FeederService} from '../../services/feeder.service';
import {Router} from '@angular/router';
import {TranslateService} from '@ngx-translate/core';
import {BsModalService} from 'ngx-bootstrap/modal';
import {FeederEntryComponent} from '../feeder-entry/feeder-entry.component';
import {ConfirmationComponent} from '../confirmation/confirmation.component';
import {ToastrService} from 'ngx-toastr';

@Component({
  selector: 'app-feeder',
  templateUrl: './feeder.component.html',
  styleUrls: ['./feeder.component.scss']
})
export class FeederComponent implements OnInit {
  @Input() feeder!: Feeder;

  lastTemperature = 0;
  lastHumidity = 0;
  lastCo = 0;

  constructor(
    private feederService: FeederService,
    private router: Router,
    private modalService: BsModalService,
    private translateService: TranslateService,
    private toastrService: ToastrService
  ) {
  }

  ngOnInit(): void {
    if (this.feeder) {
      const lastSensorMeasurement = this.feederService.getLastSensorMeasurement(this.feeder);
      if (lastSensorMeasurement) {
        this.lastTemperature = lastSensorMeasurement.temperature;
        this.lastHumidity = lastSensorMeasurement.humidity;
        this.lastCo = lastSensorMeasurement.co;
      }
    }
  }

  gotoGallery(feeder: Feeder): void {
    localStorage.setItem('feeder', JSON.stringify(feeder));
    this.router.navigate(['/gallery']).then();
  }

  edit(feeder: Feeder) {
    console.log('Modify', feeder);
    if (feeder) {
      const initialState = {
        feeder: {...feeder}
      };

      const modalRef = this.modalService.show(FeederEntryComponent, {initialState});
      if (modalRef.content) {
        modalRef.content.onValidate.subscribe((feeder: Feeder) => {
          console.log('feeder updated', feeder);
          this.feederService.updateFeeder(feeder).subscribe(
            (result: boolean) => {
              if (result && feeder) {
                console.log('Feeder updated');
                this.feeder = feeder;
                this.feederService.feederWasUpdated(feeder);
              }
              else {
                console.error('Error during feeder modification');
                const message = this.translateService.instant('erreur.mise-a-jour-mangeoire');
                this.toastrService.error(message);
              }
            },
            (error) => {
              console.error(error);
              const message = this.translateService.instant('erreur.mise-a-jour-mangeoire');
              this.toastrService.error(message);
            }
          )
        });
      }
    }
  }

  delete(feeder: Feeder) {
    console.log('Delete', feeder);
    const modalRef = this.modalService.show(ConfirmationComponent);
    if (modalRef.content) {
      modalRef.content.message = this.translateService.instant('confirmation.suppression');
      modalRef.content.onClose.subscribe((confirmation: boolean) => {
        if (confirmation) {
          this.feederService.deleteFeeder(feeder.id).subscribe(
            (result: boolean) => {
              if (result) {
                console.log('feeder deleted');
                this.feederService.feederWasDeleted(feeder);
              }
              else {
                console.error('Error during feeder deletion');
                const message = this.translateService.instant('erreur.suppression-mangeoire');
                this.toastrService.error(message);
              }
            },
            (error) => {
              console.error(error);
              const message = this.translateService.instant('erreur.suppression-mangeoire');
              this.toastrService.error(message);
            }
          );
        }
      });
    }
  }
}
