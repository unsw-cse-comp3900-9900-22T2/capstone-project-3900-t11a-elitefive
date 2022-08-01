import { Box, Typography } from '@mui/material';
import React from 'react'
import styled from 'styled-components';
import { useAuth } from '../global/GlobalAuth';
import Button from './ReusableButton';

type Props = {
  name: string;
  variant: "incoming" | "outgoing";
  refresh: () => void;
  id: string;
}

const Container = styled.div`
  width: 90%;
  margin: 10px;

  display: flex;
  justify-content: space-between;
`;

export default function FriendRequest({ id, name, variant, refresh }: Props) {

  const { getUID } = useAuth();

  const postreq = (type: string) => () => {
    const data = { action: type, from: getUID(), to: id.toString()}
    fetch(`/friend`,{
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
      },
        body: JSON.stringify(data),
    })
    .then(resp => resp.json())
    .then(data => {
      // console.log(data);
      //forces a refresh
      refresh();
    })
  }

  const renderButtons = () => {
    switch(variant) {
      case "incoming":
        return (
          <Button background="var(--accent-darker)"
            onClick={postreq("cancel")}
          >Cancel</Button>
        )
      case "outgoing":
        return (
          <Box display="flex" gap="10px">
            <Button background="var(--accent-darker)" 
              onClick={postreq("accept")}
            >Accept</Button>
            <Button background="var(--accent-darker)"
              onClick={postreq("deny")}
            >Reject</Button>
          </Box>
        )
    }
  }

  return (
    <Container>
      <Typography marginLeft={3} variant="h5">{name}</Typography>
      {renderButtons()}
    </Container>
  )
}