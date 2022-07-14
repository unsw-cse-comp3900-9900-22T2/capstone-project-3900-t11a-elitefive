import { Box, Typography } from '@mui/material';
import { minHeight } from '@mui/system';
import React, { useEffect, useState } from 'react'
import styled from 'styled-components';
import FriendListComponent from '../components/FriendListComponent';
import FriendRequest from '../components/FriendRequest';
import Button from '../components/ReusableButton';
import StyledInput from '../components/StyledInput';

type Props = {}

const Container = styled.div`
  display: flex;
`;

const Left = styled.div`
  width: 50%;
  height: 100vh;
  display: flex;
  flex-direction: column;

  margin: 50px;
  gap: 50px;
`;
const Right = styled.div`
  width: 40%;
  height: 100vh;
  display: flex;
  flex-direction: column;

  margin: 50px;
  gap: 50px;
`;

type BoxContainerProps = {
  minHeight?: number;
}
const BoxContainer = styled.div<BoxContainerProps>`
  background: var(--accent-dark);
  margin: 20px 0;
  border-radius: 5px;
  padding: 30px 0;

  ${({minHeight}) => minHeight && `
    min-height: ${minHeight}px;
  `}
`

const outgoing_mock = [
  "bob",
  "james"
]
const incoming_mock = [
  "bob",
  "james"
]

const friends_mock = [
  ":(",
  "loner",
]

export default function FriendsPage({}: Props) {
  const [friends, setFriends] = useState<string[]>([]);
  const [outgoing, setOutgoing] = useState<string[]>([]);
  const [incoming, setIncoming] = useState<string[]>([]);
  

  useEffect(() => {
    setOutgoing(outgoing_mock);
    setIncoming(incoming_mock);
    setFriends(friends_mock);
  },[])

  return (
    <Container>
      <Left>
        <Box>
          <Typography variant="h4">Friends</Typography>
          <BoxContainer minHeight={250}>
            <Box width="350px" margin="0 20px">
              <StyledInput background="var(--accent-darker)" size="small"/>
            </Box>
            {
              friends.map((name) => (
                <FriendListComponent name={name}/>
              ))
            }
          </BoxContainer>
        </Box>
        <Box>
          <Typography variant="h4">Quick Add</Typography>
          <Box display="flex" justifyContent="space-between" margin="30px 0">
            <StyledInput size="small"/>
            <Box display="flex" alignItems="center">
              <Button background='var(--accent-darker)' height={40}>add</Button>
            </Box>
          </Box>
        </Box>
      </Left>
      <Right>
        <Box>
          <Typography variant="h4">Pending</Typography>
          <BoxContainer>
            {
              outgoing.map((name) => (
                <FriendRequest name={name} variant="outgoing"/>
              ))
            }
          </BoxContainer>
        </Box>
        <Box>
          <Typography variant="h4">Requests</Typography>
          <BoxContainer>
            {
              incoming.map((name) => (
                <FriendRequest name={name} variant="incoming"/>
              ))
            }
          </BoxContainer>
        </Box>
      </Right>
    </Container>
  )
}