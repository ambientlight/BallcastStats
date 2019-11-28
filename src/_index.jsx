// polyfills
import 'babel-polyfill';
import 'whatwg-fetch';
import 'intl';

import * as React from 'react'
import { render } from 'react-dom'
import { addLocaleData } from 'react-intl'
import enLocaleData from 'react-intl/locale-data/en'
import ruLocaleData from 'react-intl/locale-data/ru'

addLocaleData(enLocaleData);
addLocaleData(ruLocaleData);

import Amplify, { Auth, Analytics, AWSKinesisProvider } from 'aws-amplify';
import awsmobile from './aws-exports';
import Root from './Root'

import { AWSAppSyncClient } from 'aws-appsync'
import ApolloClient from 'apollo-client';
import gql from 'graphql-tag'

// configure Amplify
Amplify.configure(awsmobile);

const root = document.createElement('div')
document.body.appendChild(root)
render(<Root />, root)


// Analytics.record({ name: 'albumVisit' });

// const kinesisProvider = new AWSKinesisProvider({ region: 'eu-central-1' });
// Analytics.addPluggable(kinesisProvider);
// Analytics.record({
//   data: {},
//   partitionKey: '0', 
//   streamName: 'testkinesis-euprod'
// }, 'AWSKinesis');

// kinesisProvider.record({
//   event: {
//     data: {},
//     partitionKey: '0', 
//     streamName: 'testkinesis-euprod'
//   }
// }).then(res => { 
//   console.log("GOT HERE");
// });