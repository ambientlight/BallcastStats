const gql = require('graphql-tag');

module.exports = gql`
  mutation FIXTURES_UPDATED($fixtures: [FixtureInput!]) {
    fixturesUpdated(fixtures: $fixtures){
      id,
      modelVersion,
      date,
      competitionId,
      homeTeamId,
      awayTeamId,
      startTs,
      startTime,
      status,
      scores {
        current {
          home,
          away
        },
        ht {
          home,
          away
        },
        ft {
          home,
          away
        },
        et {
          home,
          away
        },
        pen {
          home,
          away
        }
      },
      time {
        min,
        sec,
        ht_added,
        ft_added,
        fet_added,
        set_added
      },
      homeTeamName,
      awayTeamName,
      competitionName,
      homeTeamFormation,
      awayTeamFormation,
      homeTeamLineup {
        playerId,
        teamId,
        playerName,
        number,
        position,
        formationIdx,
        captain,
        posx,
        posy,
        playerIdSportmonks
      },
      awayTeamLineup {
        playerId,
        teamId,
        playerName,
        number,
        position,
        formationIdx,
        captain,
        posx,
        posy,
        playerIdSportmonks
      },
      events {
        eventType,
        playerId,
        playerName,
        relatedPlayerId,
        relatedPlayerName,
        minute,
        extraMinute,
        reason,
        result {
          home,
          away
        }
        idSportmonks
      },
      comments {
        important,
        order,
        goal,
        minute,
        extraMinute,
        comment
      },
      idSportmonks,
      dumpSportmonks {
        bucket,
        key,
        region
      }
    }
  }
`