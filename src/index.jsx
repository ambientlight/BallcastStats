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

// example to have subscriptions working
const client = new AWSAppSyncClient({
  url: awsmobile.aws_appsync_graphqlEndpoint,
  region: awsmobile.aws_appsync_region,
  auth: {
    type: awsmobile.aws_appsync_authenticationType,
    jwtToken: async () => (await Auth.currentSession()).getAccessToken().getJwtToken()
  },
  disableOffline: true
});

client.subscribe({
  query: gql`
    subscription ON_UPDATED_FIXTURES {
      onUpdatedFixtures {
        id,
        status,
        time {
          min,
          sec
        },
        scores {
          current {
            home, 
            away
          }
        }
        homeTeamName,
        awayTeamName,
        homeTeamId,
        awayTeamId
      }
    }
  `
}).subscribe({
  next: ({data: { onUpdatedFixtures }}) => {
    // onUpdatedFixtures.forEach(fx => console.log({
    //   summary: `${fx.homeTeamName} ${fx.scores.current.home} : ${fx.scores.current.away} ${fx.awayTeamName}`,
    //   time: `${fx.time.min < 10 ? '0' : ''}${fx.time.min}:${fx.time.sec < 10 ? '0' : ''}${fx.time.sec}`,
    //   status: fx.status
    // }))

    onUpdatedFixtures.forEach(fx => console.log(fx))
  }
});

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