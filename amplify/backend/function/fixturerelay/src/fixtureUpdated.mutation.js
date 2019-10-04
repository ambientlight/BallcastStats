const gql = require('graphql-tag');

module.exports = gql`
  mutation FIXTURES_UPDATED($fixtures: [FixtureInput!]) {
    fixturesUpdated(fixtures: $fixtures){
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
      }
      homeTeamName,
      awayTeamName,
    }
  }
`