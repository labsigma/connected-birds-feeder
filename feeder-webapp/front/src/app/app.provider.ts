import {APP_INITIALIZER, LOCALE_ID} from '@angular/core';
import {ConfigurationService} from './services/configuration.service';

const appInitializerFn = (appConfig: ConfigurationService) => {
    return () => {
        return appConfig.charger();
    };
};

export const providers = [
    {
        provide: APP_INITIALIZER,
        useFactory: appInitializerFn,
        multi: true,
        deps: [ConfigurationService]
    }
];
