import { Box, Typography } from '@mui/material';
import React from 'react'
import styled from 'styled-components';
import Button from './ReusableButton';

type Props = {
  name: string;
  variant: "incoming" | "outgoing";
}

const Container = styled.div`
  width: 90%;
  margin: 10px;

  display: flex;
  justify-content: space-between;
`;

export default function FriendRequest({ name, variant }: Props) {

  const renderButtons = () => {
    switch(variant) {
      case "incoming":
        return (
          <Button background="var(--accent-darker)">Cancel</Button>
        )
      case "outgoing":
        return (
          <Box display="flex" gap="10px">
            <Button background="var(--accent-darker)">Accept</Button>
            <Button background="var(--accent-darker)">Reject</Button>
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