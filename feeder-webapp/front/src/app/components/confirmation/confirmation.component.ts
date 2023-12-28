import {Component, Input, OnInit} from '@angular/core';
import {Subject} from 'rxjs';
import {BsModalRef} from 'ngx-bootstrap/modal';

@Component({
  selector: 'app-confirmation',
  templateUrl: './confirmation.component.html',
  styleUrls: ['./confirmation.component.scss']
})
export class ConfirmationComponent implements OnInit {
  // @ts-ignore
  @Input() message: string;

  // @ts-ignore
  public onClose: Subject<boolean>;

  ngOnInit(): void {
    this.onClose = new Subject();
  }

  constructor(private bsModalRef: BsModalRef) { }

  reset(): void {
    this.onClose.next(false);
    this.bsModalRef.hide();
  }


  confirm(): void {
    this.onClose.next(true);
    this.bsModalRef.hide();
  }
}
