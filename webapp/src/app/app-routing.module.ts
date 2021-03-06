import { NgModule } from '@angular/core';
import { Routes, RouterModule } from '@angular/router';

import { HomeComponent } from './home/home.component';
import { SdcardComponent } from './sdcard/sdcard.component';
import { StatusComponent } from './status/status.component';
import { WifiComponent } from './wifi/wifi.component';
import { DevicesComponent } from './devices/devices.component';
import { AboutComponent } from './about/about.component';
import { NotfoundComponent } from './notfound/notfound.component';

const routes: Routes = [
  { path: 'home', component: HomeComponent },
  { path: 'sdcard', component: SdcardComponent },
  { path: 'status', component: StatusComponent },
  { path: 'wifi', component: WifiComponent },
  { path: 'devices', component: DevicesComponent },
  { path: 'about', component: AboutComponent },
  { path: '', redirectTo: '/home', pathMatch: 'full' },
  { path: '**', component: NotfoundComponent }
];

@NgModule({
  imports: [RouterModule.forRoot(routes)],
  exports: [RouterModule]
})
export class AppRoutingModule { }
