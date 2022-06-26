import React from 'react';
import styled from 'styled-components';
import { StyledButton, buttonProps } from './ReusableButton-styled';
import { TextField, Typography } from '@mui/material'; 


type Props = {
  children?: React.ReactElement | any;
  onClick?: (...args: any) => void;
  label: string;
} & buttonProps;

const Container = styled.div`
  display:flex;
  flex-direction: column;
`;

const StyledTextField = styled(TextField)`
  background: var(--textbox-dark);
  width: 400px;
`;

export default function Button({ label, children, onClick, ...styleProps }: Props) {
  return (
    <StyledButton
      onClick={onClick}
      {...styleProps}
    >
      {children}
      <Typography variant="subtitle1">{label}</Typography>
    </StyledButton>
  );
}
