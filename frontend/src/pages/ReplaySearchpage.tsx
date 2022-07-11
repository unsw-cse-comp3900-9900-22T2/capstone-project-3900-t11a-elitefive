import React from 'react'
import styled from 'styled-components';
import { Box, Typography } from '@mui/material';
import ReplayPreview from '../components/ReplayPreview';
import FilterBar from '../components/FilterBar';

type Props = {}

const mockData = [
  {
    mode: "Ranked",
    result: "win",
    date: "15/3/2022",
    img: "",
    players: [] // todo
  },
  {
    mode: "Casual",
    result: "loss",
    date: "15/3/2022",
    img: "",
    players: [] // todo
  },
  {
    mode: "Casual",
    result: "loss",
    date: "15/3/2022",
    img: "",
    players: [] // todo
  },
  {
    mode: "Casual",
    result: "loss",
    date: "15/3/2022",
    img: "",
    players: [] // todo
  }
]

const MainContainer = styled.div`
  width: 90vw;
  margin: 40px 70px;

  display: flex;
  flex-direction: column;
`;

const ReplaysContainer = styled.div`
  display: flex;
  flex-wrap: wrap;
`;

const SideBarContainer = styled.div`
  background: var(--accent-dark);
  width: 20vw;
  margin: 120px 80px;
`;

export default function ReplaySearchpage({}: Props) {
  return (
    <Box
      display="flex"
      width="100vw"
      minHeight="100vh"
    >
      <MainContainer>
        <Box margin="30px 50px">
          <Typography variant="h4">Replays</Typography>
          <FilterBar/>
        </Box>
        <ReplaysContainer>
          {mockData.map((data) => (
            <ReplayPreview {...data}/>
          ))}
        </ReplaysContainer>
      </MainContainer>
      <SideBarContainer>
        bye
      </SideBarContainer>
    </Box>
  )
}