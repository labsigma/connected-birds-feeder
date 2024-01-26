import {Component, Input, OnInit} from '@angular/core';
import {Feeder} from '../../interfaces/feeder';
import {Subject} from 'rxjs';
import {BsModalRef} from 'ngx-bootstrap/modal';
import {NgForm} from '@angular/forms';

@Component({
  selector: 'app-feeder-entry',
  templateUrl: './feeder-entry.component.html',
  styleUrls: ['./feeder-entry.component.scss']
})
export class FeederEntryComponent implements OnInit {
  @Input() feeder: Feeder | undefined;
  @Input() creation: boolean = false;
  // @ts-ignore
  public onValidate: Subject<Feeder>;

  constructor(
    private bsModalRef: BsModalRef
  ) {}

  ngOnInit(): void {
    this.onValidate = new Subject();
  }

  validate(form: NgForm): void {
    if (!form.valid) {
      Object.keys(form.controls).forEach(field => {
        const control = form.controls[field];
        if (!control.valid) {
          console.log(field);
        }
        control.markAsTouched({onlySelf: true});
      });
    }
    if (this.onValidate && (form.valid)) {
      this.onValidate.next(this.feeder);
      this.bsModalRef.hide();
    }
  }

  close(): void {
    this.bsModalRef.hide();
  }
}
