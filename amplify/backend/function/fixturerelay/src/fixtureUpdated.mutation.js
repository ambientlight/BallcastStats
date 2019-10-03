const gql = require('graphql-tag');

// {
//   id:"1",
//   modelVersion:0,
//   date:"2019-09-24",
//   competitionId:"1",
//   homeTeamId:"1",
//   awayTeamId:"2",
//   startTs:1569175200,
//   startTime:"2019-10-02T19:00:00.000Z",
//   status:NS,
//   homeTeamName:"Mjondalen",
//   awayTeamName:"Rosenborg",
//   competitionName:"Eliteserien"
// }

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