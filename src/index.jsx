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

import Amplify, { Auth } from 'aws-amplify';
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

/*
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
        scores {
          current {
            home,
            away
          }
        },
        time {
          min,
          sec
        },
        homeTeamName,
        awayTeamName
      }
    }
  `
}).subscribe({
  next: value => console.log(value)
})
*/